


demo.html 

```html
<body>
    <script src="https://unpkg.com/wavesurfer.js"></script>
    <div id="waveform"></div>
    <button onclick="load()">加载</button>
    <button onclick="play()">播放</button>
    <script>
        var wavesurfer = WaveSurfer.create({
            container: '#waveform',
            waveColor: 'violet',
            progressColor: 'purple'
        });

        function load() {
            console.log("load")
            wavesurfer.load('./test.mp3');
        }

        function play() {
            console.log("play")
            if(!wavesurfer.isPlaying())
                wavesurfer.play();
            else
                wavesurfer.pause();
        }
    </script>
</body>
```
