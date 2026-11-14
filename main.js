const { app, BrowserWindow } = require('electron');
const http = require('http');
const fs = require('fs');
const path = require('path');

let server;
let mainWindow;

function startLocalServer(callback) {
  const htmlPath = path.join(__dirname, 'HYD.141126.html');
  const htmlContent = fs.readFileSync(htmlPath);

  server = http.createServer((req, res) => {
    res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
    res.end(htmlContent);
  });

  server.listen(47126, '127.0.0.1', () => {
    callback('http://127.0.0.1:47126/');
  });

  server.on('error', () => {
    server.listen(0, '127.0.0.1', () => {
      const addr = server.address();
      callback(`http://127.0.0.1:${addr.port}/`);
    });
  });
}

function createWindow(url) {
  mainWindow = new BrowserWindow({
    width: 1366,
    height: 820,
    minWidth: 1024,
    minHeight: 680,
    title: 'SRIRAM GEMS & JEWELLERY',
    icon: path.join(__dirname, 'app_icon.ico'),
    autoHideMenuBar: true,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true
    }
  });

  mainWindow.setMenuBarVisibility(false);
  mainWindow.loadURL(url);

  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}

app.whenReady().then(() => {
  startLocalServer((url) => {
    createWindow(url);
  });
});

app.on('window-all-closed', () => {
  if (server) {
    try { server.close(); } catch (_) {}
  }
  app.quit();
});
