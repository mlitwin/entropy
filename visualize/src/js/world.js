import * as bootstrap from "bootstrap";
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
{{#if this.level}}
<button class="list-group-item list-group-item-action" data-file="{{this.file}}">{{this.level}}</button>
{{else}}
<button class="list-group-item list-group-item-action  data-file="{{this.file}} active aria-current="true">{{this.level}}</button>

{{/if}}
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
      detail: button.dataset.file,
    });
    button.dispatchEvent(event);
  });

  return world;
}

export { createWorld };
