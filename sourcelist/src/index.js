import * as fzstd from "fzstd";
import fs from "fs";

const style = new TextDecoder("utf-8").decode(
  fzstd.decompress(fs.readFileSync(__dirname + "/style.css.zst")),
);
const sources = new TextDecoder("utf-8").decode(
  fzstd.decompress(fs.readFileSync(__dirname + "/sources.html.zst")),
);
const a = fzstd.decompress(fs.readFileSync(__dirname + "/logo.ico.zst"));
const logo = "data:image/x-icon;base64," +
  btoa(
    Array(a.length).fill("").map((_, i) => String.fromCharCode(a[i])).join(""),
  );

const app = document.createElement("div");
app.id = "app";
app.innerHTML = sources;

document.body = document.createElement("body");
document.body.appendChild(app);

const title = document.createElement("title");
title.innerHTML = "AmplOS2 SourceList";

const sheet = document.createElement("style");
sheet.textContent = style;

const viewport = document.createElement("meta");
viewport.name = "viewport";
viewport.content = "width=device-width, initial-scale=1";

const favicon = document.createElement("link");
favicon.rel = "icon";
favicon.type = "image/x-icon";
favicon.href = logo;

document.head.appendChild(title);
document.head.appendChild(sheet);
document.head.appendChild(viewport);
document.head.appendChild(favicon);
