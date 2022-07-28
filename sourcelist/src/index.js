import sources from "bundle-text:./sources.html";
import style from "bundle-text:./style.css";
import logo from "data-url:../../logo.ico";

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
