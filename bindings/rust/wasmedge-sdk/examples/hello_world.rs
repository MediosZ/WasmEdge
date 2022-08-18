// If the version of rust used is less than v1.63, please uncomment the follow attribute.
// #![feature(explicit_generic_args_with_impl_trait)]

use wasmedge_sdk::{Executor, ImportObjectBuilder, Module, Store};
use wasmedge_types::wat2wasm;

#[cfg_attr(test, test)]
#[tokio::main]
async fn main() -> anyhow::Result<()> {
    // We define a function to act as our "env" "say_hello" function imported in the
    // Wasm program above.

    // create a store
    let mut store = Store::new()?;

    // create an import module
    let import = ImportObjectBuilder::new()
        .with_func_async::<(), ()>(&mut store, "say_hello", move |_caller, _params| {
            Box::new(async {
                println!("Hello, world!");
                tokio::time::sleep(std::time::Duration::from_secs(1)).await;
                println!("Hello, world after sleep!");
                Ok(vec![])
            })
        })?
        .build("env")?;

    let wasm_bytes = wat2wasm(
        br#"
    (module
      ;; First we define a type with no parameters and no results.
      (type $no_args_no_rets_t (func (param) (result)))
    
      ;; Then we declare that we want to import a function named "env" "say_hello" with
      ;; that type signature.
      (import "env" "say_hello" (func $say_hello (type $no_args_no_rets_t)))
    
      ;; Finally we create an entrypoint that calls our imported function.
      (func $run (type $no_args_no_rets_t)
        (call $say_hello))
      ;; And mark it as an exported function named "run".
      (export "run" (func $run)))
    "#,
    )?;

    // loads a wasm module from the given in-memory bytes
    let module = Module::from_bytes(None, &wasm_bytes)?;

    // create an executor
    let mut executor = Executor::new(None, None)?;

    // register the module into the store
    store.register_import_module(&mut executor, &import)?;

    // register the compiled module into the store and get an module instance
    let extern_instance = store.register_named_module(&mut executor, "extern", &module)?;

    // get the exported function "run"
    let run = extern_instance
        .func("run")
        .ok_or_else(|| anyhow::Error::msg("Not found exported function named 'run'."))?;

    // run host function
    let handle = tokio::spawn(async move {
        let _ = run.call_async(&mut store, &mut executor, vec![]).await;
        println!("run done");
    });
    // run.call_async(&mut store, &mut executor, vec![]).await;
    // tokio::time::sleep(std::time::Duration::from_secs(1)).await;
    println!("running ...");
    handle.await?;
    Ok(())
}
