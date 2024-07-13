// ==UserScript==
// @name         国家开放大学视频自动播放
// @namespace    http://tampermonkey.net/
// @version      2024-05-27
// @description  try to take over the world!
// @author       You
// @match        https://lms.ouchn.cn/course/*
// @require      https://ajax.aspnetcdn.com/ajax/jQuery/jquery-3.7.1.js
// @icon         data:image/gif;base64,R0lGODlhAQABAAAAACH5BAEKAAEALAAAAAABAAEAAAICTAEAOw==
// @grant        none
// ==/UserScript==

(function() {
    'use strict';


    let curSecondsNoChangeCount = 0
    let curSecondsOld = 0

    setInterval (() => {
        console.log("引入完成")

        const currentTime = $('.mvp-time-display span').eq(0).text();
        const totalTime = $('.mvp-time-display span.duration').text();

        // 将时间字符串分割为小时、分钟和秒的数组
        function timeStringToSeconds(timeStr) {
            const parts = timeStr.split(':').map(Number);
            let totalSeconds = 0;

            if (parts.length === 1) {
                // 只有秒的情况
                totalSeconds = parts[0];
            } else if (parts.length === 2) {
                // 只有分钟和秒的情况
                totalSeconds = parts[0] * 60 + parts[1];
            } else if (parts.length === 3) {
                // 包含小时、分钟和秒的情况
                totalSeconds = parts[0] * 3600 + parts[1] * 60 + parts[2];
            }

            return totalSeconds;
        }

        // 判断是否播放界面
        if (currentTime.length > 0) {

            let playBtn = $(".mvp-toggle-play")
            let btnType = playBtn.find("i").attr('class')

            let curSeconds = timeStringToSeconds(currentTime)
            let totalSeconds = timeStringToSeconds(totalTime)
            console.log(curSeconds, totalSeconds)

            // 时间还有则播放
            if (curSeconds < totalSeconds && btnType == "mvp-fonts mvp-fonts-play") {
                console.log("Click Play")
                playBtn.click()
                curSecondsNoChangeCount = 0
            }

            // 已经播放了
            else if (btnType == "mvp-fonts mvp-fonts-pause") {
                // 若倍率不对，则加速
                let currentSpeed = $(".mvp-play-rate.active").text().trim()
                console.log("currentSpeed ", currentSpeed)

                if (currentSpeed != "2.0X") {
                    $('.mvp-play-rate:contains("2.0X")').click()
                    console.log("Change Speed to 2.0X ")
                }
            }

            // 判断播放时间没有变化则重新点击播放按钮
            if (curSecondsOld == curSeconds) {
                curSecondsNoChangeCount += 1
                if (curSecondsNoChangeCount > 3) {
                    playBtn.click()
                    curSecondsNoChangeCount = 0
                }
                console.log("curSecondsNoChangeCount", curSecondsNoChangeCount)
            }
            curSecondsOld = curSeconds

            // 静音播放
            let volumeBtn = $(".mvp-volume-control-btn")
            let volumeType = volumeBtn.find("i").attr("class");
            if (volumeType == "mvp-fonts mvp-fonts-volume-on") {
                console.log("volume-off")
                volumeBtn.click()
            }

            // 播放完成则下一页
            if (curSeconds == totalSeconds) {
                let nextBtn = $('.next-btn')
                console.log("Next")
                nextBtn.click()
            }
        }

        else {
            let nextBtn = $('.next-btn')
            console.log("Next")
            nextBtn.click()
        }

    }, 3000);

    // Your code here...
})();

