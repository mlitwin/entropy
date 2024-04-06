import * as THREE from "three";

function initVisualization(world) {
  async function getFile(file) {
    const data = await fetch(file);
    return await data.json();
  }

  let curLevel;

  function createLevel(sourceData, level, width, height) {
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
    s2.position.set(0, 0, 0);
    if (curLevel) {
      scene.remove(curLevel);
    }
    scene.add(s2);
    curLevel = s2;
  }

  const container = document.getElementById("world");

  const scene = new THREE.Scene();
  const camera = new THREE.PerspectiveCamera(
    120,
    container.scrollWidth / container.scrollHeight,
    0.1,
    100
  );

  const renderer = new THREE.WebGLRenderer();
  renderer.setSize(container.scrollWidth, container.scrollHeight);
  container.appendChild(renderer.domElement);

  camera.position.x = 0;
  camera.position.y = 0;
  camera.position.z = 2.5;

  function createDataArray(data, width, height) {
    const d = new Uint8Array(4 * width * height);

    for (let t = 0; t < height; t++) {
      for (let i = 0; i < width; i++) {
        let val = data.d[t][i];
        //const p = Math.floor(data.p[t][i] * 255);
        const stride = (t * height + i) * 4;
        val *= 1171;
        d[stride] = val % 255;
        d[stride + 1] = (2 * val) % 255;
        d[stride + 2] = (val + 100) % 255;
        d[stride + 3] = 255;
      }
    }

    return d;
  }

  camera.lookAt(new THREE.Vector3(0, 0, 0));

  function animate() {
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
    //scene.rotateZ(0.0002);
    // scene.rotateX(0.0001);
    //s2.rotation.z += 0.0002;
    // s2.rotation.y += 0.0001;
  }
  animate();
  async function loadLevel(index) {
    const entry = world.levels[index];
    const data = await getFile(entry.file);
    createLevel(data, entry.level, world.width, world.height);
  }

  return {
    loadLevel,
  };
}

export { initVisualization };
