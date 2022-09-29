use std::{
    cell::UnsafeCell,
    future::Future,
    pin::Pin,
    ptr,
    task::{Context, Poll},
};

use wasmtime_fiber::Fiber;
pub struct Reset<T: Copy>(pub *mut T, pub T);

impl<T: Copy> Drop for Reset<T> {
    fn drop(&mut self) {
        unsafe {
            *self.0 = self.1;
        }
    }
}

pub struct FiberFuture<'a> {
    fiber: wasmtime_fiber::Fiber<'a, Result<(), ()>, (), Result<(), ()>>,
    current_poll_cx: *mut *mut Context<'static>,
    // engine: Engine,
}

impl<'a> FiberFuture<'a> {
    pub fn new(
        fiber: wasmtime_fiber::Fiber<'a, Result<(), ()>, (), Result<(), ()>>,
        current_poll_cx: *mut *mut Context<'static>,
    ) -> Self {
        FiberFuture {
            fiber,
            current_poll_cx,
        }
    }
}

unsafe impl Send for FiberFuture<'_> {}

impl Future for FiberFuture<'_> {
    type Output = Result<(), ()>;

    fn poll(mut self: Pin<&mut Self>, cx: &mut Context) -> Poll<Self::Output> {
        unsafe {
            let _reset = Reset(self.current_poll_cx, *self.current_poll_cx);
            *self.current_poll_cx =
                std::mem::transmute::<&mut Context<'_>, *mut Context<'static>>(cx);
            match self.fiber.resume(Ok(())) {
                Ok(result) => Poll::Ready(result),
                Err(()) => Poll::Pending,
            }
        }
    }
}

impl Drop for FiberFuture<'_> {
    fn drop(&mut self) {
        if !self.fiber.done() {
            let result = self.fiber.resume(Err(()));
            // This resumption with an error should always complete the
            // fiber. While it's technically possible for host code to catch
            // the trap and re-resume, we'd ideally like to signal that to
            // callers that they shouldn't be doing that.
            debug_assert!(result.is_ok());
        }
    }
}
