mkdir cbuild
cd cbuild
cmake -DCMAKE_BUILD_TYPE=Release -DWASMEDGE_BUILD_AOT_RUNTIME=OFF -DWASMEDGE_BUILD_TESTS=OFF ..
make -j8 
cd .. 
# populate env variables
export WASMEDGE_DIR="$(pwd)"
export WASMEDGE_BUILD_DIR="$(pwd)/cbuild"
export WASMEDGE_PLUGIN_PATH="$(pwd)/cbuild/plugins/wasmedge_process"
export LD_LIBRARY_PATH="$(pwd)/cbuild/lib/api"
# run it!
cd examples/challenge
cargo run 