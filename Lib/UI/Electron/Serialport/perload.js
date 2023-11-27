const { contextBridge, ipcRenderer } = require('electron')

// 注册函数
contextBridge.exposeInMainWorld('electron', {
    myButtonIsClicked: (text) => {
        // 发送一个点击事件
        ipcRenderer.send('myOnclick', text)
    },

    serialDataRecvWait: (callback) => {
        ipcRenderer.on('serialData', (event, data) => {
            callback(data)
        })
    }
})
