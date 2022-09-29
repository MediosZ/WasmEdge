use wasmedge_sys::{Executor, FuncType, Function, Store, WasmValue};

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // create a FuncType
    let func_ty = FuncType::create(vec![], vec![])?;
    let mut store = Store::create().expect("123");
    // create a host function
    let async_host_func = Function::create_async(
        &mut store,
        &func_ty,
        move |_caller, input| {
            Box::new(async {
                println!("Hello, world!");
                tokio::time::sleep(std::time::Duration::from_secs(3)).await;
                // let (tx, rx) = tokio::sync::oneshot::channel::<i32>();
                // tx.send(()).unwrap();

                // Wrap the future with a `Timeout` set to expire in 10 milliseconds.
                // if let Err(_) = tokio::time::timeout(std::time::Duration::from_millis(100), async {
                //     tokio::time::sleep(std::time::Duration::from_secs(1)).await;
                // })
                // .await
                // {
                //     println!("did not receive value within 100 ms");
                // }
                println!("Rust: Leaving Rust function real_add");
                println!("Hello, world after sleep!");
                Ok(vec![])
            })
        },
        0,
    )?;

    // run this function
    let mut executor = Executor::create(None, None)?;

    let res = async_host_func
        .call_async(&mut store, &mut executor, vec![])
        .await;
    dbg!(res);
    Ok(())
}
