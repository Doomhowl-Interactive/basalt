// reference: https://github.com/tsoding/rust-browser-game
function make_environment(...envs) {
    return new Proxy(envs, {
        get(target, prop, receiver) {
            for (let env of envs) {
                if (env.hasOwnProperty(prop)) {
                    return env[prop];
                }
            }
            return (...args) => {
                console.error("NOT IMPLEMENTED: " + prop, args);
            };
        },
    });
}

const libm = {
    atan2f: Math.atan2,
    cosf: Math.cos,
    sinf: Math.sin,
    sqrtf: Math.sqrt,
    printf: console.log
};

async function start() {
    let game = null;
    try {
        game = await WebAssembly.instantiateStreaming(fetch("build/basalt_wasm.wasm"), {
            env: make_environment(libm),
        });
        console.log("Loaded wasm binary...");
    } catch (e) {
        console.log(`Failed to load wasm binary\n${e}...`);
        return;
    }

    // check wasm integrity
    const lifeUniverse = game.instance.exports.LifeAndTheUniverse();
    if (lifeUniverse != 42) {
        console.warn("Binary is probably corrupted!");
    }

    if (game.instance.exports.InitWASM()) {
        console.info("Initialized WASM game!");
    } else {
        console.error("Failed to initialize WASM game!");
    }

    const displayAddr = game.instance.exports.GetWASMCanvas();
    if (displayAddr === 0){
        console.error("Memory address of canvas is NULL (0)!");
    }
    console.log("Memory address of canvas:", displayAddr);

    const width = game.instance.exports.GetWASMCanvasWidth();
    const height = game.instance.exports.GetWASMCanvasHeight();
    console.log("Game size from WASM", width, height);
    const size = width * height;

    const canvas = document.getElementById("canvas");
    canvas.width = width;
    canvas.height = height;
    const ctx = canvas.getContext("2d");

    while (true) {
        // draw a frame
        const delta = 1.0 / 60; // TODO: implement prober deltatime
        const frameNum = game.instance.exports.UpdateAndRenderWASM(delta);
        const memory = new Uint8Array(game.instance.exports.memory.buffer);
        const pixels = new Uint8ClampedArray(memory.subarray(displayAddr, displayAddr + 4*width*height));
        const frame = new ImageData(pixels, width, height);
        ctx.putImageData(frame, 0, 0);
        await new Promise((r) => setTimeout(r, 10));
    }
}

addEventListener("DOMContentLoaded", () => {
    start().catch((e) => console.error(e));
});
