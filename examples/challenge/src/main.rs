use std::ffi::CString;

pub mod challenge {
    use std::os::raw::c_char;
    #[link(wasm_import_module = "host_function_challenge")]
    extern "C" {
        pub fn host_function_challenge_set_class_id(cid: u32);
        pub fn host_function_challenge_add_student(student: *const c_char, len: u32) -> u32;
        pub fn host_function_challenge_set_class_name(name: *const c_char, len: u32);
        pub fn host_function_challenge_print();
    }
}

pub fn set_class_id(cid: u32) {
    unsafe {
        challenge::host_function_challenge_set_class_id(cid);
    }
}

pub fn set_class_name<S: AsRef<str>>(name: S) {
    let name = CString::new((name.as_ref()).as_bytes()).expect("");
    unsafe {
        challenge::host_function_challenge_set_class_name(
            name.as_ptr(),
            name.as_bytes().len() as u32,
        );
    }
}

pub fn add_student<S: AsRef<str>>(name: S) -> u32 {
    let name = CString::new((name.as_ref()).as_bytes()).expect("");
    let student_size: u32;
    unsafe {
        student_size = challenge::host_function_challenge_add_student(
            name.as_ptr(),
            name.as_bytes().len() as u32,
        );
    }
    student_size
}

pub fn print() {
    unsafe {
        challenge::host_function_challenge_print();
    }
}

fn main() {
    set_class_id(123);
    set_class_name("WasmEdge");
    let mut num: u32 = add_student("Alice");
    println!("After adding Alice, num = {:?}", num);
    num = add_student("Fred");
    println!("After adding Fred, num = {:?}", num);
    num = add_student("Terry");
    println!("After adding Terry, num = {:?}", num);
    print();
}
