const path = require("path");

module.exports = {
  // base: "https://mlitwin.github.io/entropy/",
  root: path.resolve(__dirname, "src"),
  resolve: {
    alias: {
      "~bootstrap": path.resolve(__dirname, "node_modules/bootstrap"),
    },
  },
  build: {
    outDir: "../docs",
    target: "esnext",
  },
};
