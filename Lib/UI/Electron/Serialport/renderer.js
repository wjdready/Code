

// 设置点击事件
document.getElementById('btn').onclick = (event) => {
    console.log("btn onclick")
    event.preventDefault()

    let text = document.getElementById('text').value
    document.getElementById('text').value = ''
    // 调用该函数, 需在 perload.js 预先注册好
    window.electron.myButtonIsClicked(text)
}

document.getElementById('text').addEventListener('keyup', function (event) {
    // 检查按下的键是否是回车键
    if (event.keyCode === 13) {
        let text = document.getElementById('text').value
        window.electron.myButtonIsClicked(text)
        document.getElementById('text').value = ''
    }
});

window.electron.serialDataRecvWait((data) => {
    textarea = document.getElementById('text_area')
    textarea.value += data
    textarea.scrollTop = textarea.scrollHeight
})
