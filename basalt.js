// reference: https://github.com/tsoding/rust-browser-game

async function start() {
    let game = null
    try {
        game = await WebAssembly.instantiateStreaming(fetch("build/basalt_wasm.wasm"));
        console.log("Loaded wasm binary...");
    } catch (e){
        console.log(`Failed to load wasm binary\n${e}...`);
        return;
    }

    // check wasm integrity
    const lifeUniverse = game.instance.exports.LifeAndTheUniverse();
    assert(lifeUniverse == 42);

    console.log(game.instance.exports);

    const memory = new Uint8Array(game.instance.exports.memory.buffer);

    game.instance.exports.InitWASM();

    const displayAddr = game.instance.exports.get_display();
    console.log("Memory address of display:", displayAddr);
    console.log("Memory:", memory.length);

    const width = game.instance.exports.get_width();
    const height = game.instance.exports.get_height();
    console.log("Game size from WASM", width, height);
    const size = width * height;

    const canvas = document.getElementById("canvas");
    const ctx = canvas.getContext("2d");

    let curFrame = 0;

    //while (true) {
    //    // draw a frame
    //    game.instance.exports.step(0.1, curFrame);
    //    const pixels = new Uint8ClampedArray(
    //        memory.subarray(displayAddr, displayAddr + 4 * size)
    //    );
    //    const frame = new ImageData(pixels, width, height);
    //    ctx.putImageData(frame, 0, 0);
    //    curFrame += 1;
    //    // await new Promise(r => setTimeout(r, 10));
    //}
}
start().catch((e) => console.error(e));
