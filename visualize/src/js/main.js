import { createWorld } from "./world";
import { initVisualization } from "./visualize";

document.addEventListener("level", (e) => {
  console.log(e.detail);
});

const world = await createWorld();

await initVisualization(world);
