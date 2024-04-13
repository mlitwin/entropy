import "../scss/styles.scss";
//import * as bootstrap from "bootstrap";
import * as Handlebars from "handlebars";

async function getFile(file) {
  const data = await fetch(file);
  return await data.json();
}

const header = Handlebars.compile(`
<div>n: {{n}}</div>
<div>v: {{v}}</div>
<div>width: {{width}}</div>
<div>height: {{height}}</div>
`);

const levelChooser = Handlebars.compile(`
{{#each levels}}
<button class="list-group-item list-group-item-action" data-level="{{this.level}}">
  level: {{this.level}}
  num_states: {{this.num_states}}
  shannon_entropy: {{this.shannon_entropy}}
  time_jitter: {{this.time_jitter}}</button>
{{/each}}

`);

async function createWorld() {
  const world = await getFile("world.json");

  document.getElementById("header").innerHTML = header(world);
  const levelChooserEl = document.getElementById("levelChooser");
  levelChooserEl.innerHTML = levelChooser(world);
  levelChooserEl.addEventListener("click", (e) => {
    const button = e.target.closest("button");
    const event = new CustomEvent("level", {
      bubbles: true,
      detail: button.dataset.level,
    });
    levelChooserEl.querySelectorAll("button").forEach((button) => {
      button.classList.remove("active");
      button.removeAttribute("aria-current");
    });
    button.classList.add("active");
    button.setAttribute("aria-current", true);
    button.dispatchEvent(event);
  });

  return world;
}

export { createWorld };
