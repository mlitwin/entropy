import { createWorld } from "./world";
import { initVisualization } from "./visualize";

const world = await createWorld();

const viz = initVisualization(world);

document.addEventListener("level", (e) => {
  viz.loadLevel(e.detail);
});
