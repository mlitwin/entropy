import * as THREE from "three";

const loader = new THREE.TextureLoader();
const img = loader.load("/x.jpg");
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
scene.add(sphere);

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

camera.lookAt(new THREE.Vector3(0, 0, 0));

function animate() {
  requestAnimationFrame(animate);
  renderer.render(scene, camera);
  sphere.rotation.x += 0.01;
  //sphere.rotation.y += 0.01;
}
animate();
