import * as THREE from "three";

async function getFile(file) {
  const data = await fetch(file);
  return await data.json();
}

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(
  120,
  window.innerWidth / window.innerHeight,
  0.1,
  100
);

const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

camera.position.x = 0;
camera.position.y = 0;
camera.position.z = 1;

function createDataArray(data, width, height) {
  const d = new Uint8Array(4 * width * height);

  for (let t = 0; t < height; t++) {
    for (let i = 0; i < width; i++) {
      const val = Math.floor(data.d[t][i] * 255);
      const p = Math.floor(data.p[t][i] * 255);
      const stride = (t * height + i) * 4;
      d[stride] = val;
      d[stride + 1] = 0;
      d[stride + 2] = p;
      d[stride + 3] = 255;
    }
  }

  return d;
}

async function loadLevel(sourceData, level, width, height) {
  const data = createDataArray(sourceData, width, height);

  // used the buffer to create a DataTexture
  const texture = new THREE.DataTexture(data, width, height);
  texture.wrapS = THREE.RepeatWrapping;
  texture.wrapT = THREE.RepeatWrapping;
  texture.needsUpdate = true;

  //const g2 = new THREE.SphereGeometry(1, 36, 16);
  const g2 = new THREE.TorusGeometry(3, 0.3, 16, 100);
  const m2 = new THREE.MeshBasicMaterial();
  m2.map = texture;
  const s2 = new THREE.Mesh(g2, m2);
  //s2.rotateX(Math.PI * 0.4);
  //s2.rotateY(Math.PI / 2);
  //s2.rotateZ(Math.PI / 4);

  //s2.position.set(0, level - 1, 0);
  s2.position.set(0, 4, 0);
  scene.add(s2);
}

const world = await getFile("world.json");

camera.lookAt(new THREE.Vector3(0, 10, 0));

function animate() {
  requestAnimationFrame(animate);
  renderer.render(scene, camera);
  //s2.rotation.z += 0.0002;
  // s2.rotation.y += 0.0001;
}
animate();

const worldNum = Math.floor(world.levels.length / 2);

const entry = world.levels[900];
const data = await getFile(entry.file);
await loadLevel(data, entry.level, world.width, world.height);
