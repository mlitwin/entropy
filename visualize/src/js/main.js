import "../scss/styles.scss";
import { createWorld } from "./world";
import { initVisualization } from "./visualize";
import * as THREE from "three";

async function getFile(file) {
  const data = await fetch(file);
  return await data.json();
}

document.addEventListener("level", (e) => {
  console.log(e.detail);
});

const world = await createWorld();

await initVisualization(world);
