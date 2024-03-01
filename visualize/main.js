import * as THREE from "three";

const loader = new THREE.TextureLoader();
const img = loader.load(import.meta.env.BASE_URL + "x.jpg");
const material_one = new THREE.MeshPhongMaterial();
material_one.color = new THREE.Color(0xffffff);
material_one.map = img;
//material_one.transparent = true;

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(
  75,
  window.innerWidth / window.innerHeight,
  0.1,
  100
);

const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

scene.background = img;

const geometry = new THREE.SphereGeometry(1, 36, 16);
const material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
const sphere = new THREE.Mesh(geometry, material_one);
//scene.add(sphere);

// Lights
const sun = new THREE.DirectionalLight(0xffffff, 3);
sun.position.set(4, 3, 2);
scene.add(sun);

const moon = new THREE.DirectionalLight(0xffffff, 1);
moon.position.set(-4, -3, -2);
scene.add(moon);

camera.position.x = 0;
camera.position.y = 0;
camera.position.z = 5;

// create a buffer with color data

const width = 512 * 2;
const height = 512 * 2;

const size = width * height;
const data = new Uint8Array(4 * size);
const color = new THREE.Color(0xffffff);

const r = Math.floor(color.r * 255);
const g = Math.floor(color.g * 255);
const b = Math.floor(color.b * 255);

for (let i = 0; i < size; i++) {
  const stride = i * 4;
  const v = i * (size - i);
  data[stride] = (255 * (v % 10)) / 10;
  data[stride + 1] = 0;
  data[stride + 2] = 0;
  data[stride + 3] = 200;
}

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
s2.rotateX(Math.PI / 2);
//s2.rotateY(Math.PI / 2);
//s2.rotateZ(Math.PI / 4);

//s2.position.set(0, 0, 1);
scene.add(s2);

camera.lookAt(new THREE.Vector3(0, 0, 0));

function animate() {
  requestAnimationFrame(animate);
  renderer.render(scene, camera);
  s2.rotation.z += 0.0002;
  //sphere.rotation.y += 0.01;
}
animate();
