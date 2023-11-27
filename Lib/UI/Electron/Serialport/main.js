const { app, BrowserWindow, ipcMain } = require('electron')
const path = require('node:path')
const serialPort = require('serialport')

let win = null
let port = null

function serialDemo() {
    port = new serialPort.SerialPort({ path: 'COM13', baudRate: 115200 })
    console.log('Port opened')
    // 监听串口数据
    port.on('data', (data) => {
        process.stdout.write(data)
        win.webContents.send('serialData', data.toString('utf8'))
    });
}

function createWindow() {
    win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            preload: path.join(__dirname, 'perload.js')
        }
    })

    win.loadFile('index.html')

    //打开开发者工具
    // win.webContents.openDevTools();
}

app.whenReady().then(() => {
    createWindow()
    
    serialDemo()

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow()
        }
    })
})

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit()
    }
})

ipcMain.on('myOnclick', (event, text) => {
    console.log(text)
    // 发送数据到串口
    port.write(text + '\n')
})
