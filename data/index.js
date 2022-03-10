let lbl_dir;
let lbl_spd;
let dir;
let spd;

function init(){
    lbl_dir = document.getElementById('dir')
    lbl_spd = document.getElementById('spd')
    dir = 'stop'
    spd = 50;

    document.addEventListener('keydown', handler)
    document.addEventListener('keyup', handler_up)
    document.addEventListener('wheel', function (event) {
        spd = add_constrain(spd, (event.deltaY < 0) ? 10 : -10)
        update()
    })
    update()
}

function handler(event){
    if (event.key == 'w')
        dir = 'forward'
    else if (event.key == 's')
        dir = 'backward'
    else if (event.key == 'a')
        dir = 'left'
    else if (event.key == 'd')
        dir = 'right'
    else if (event.key == 'ArrowUp')
        spd = add_constrain(spd, 10)
    else if (event.key == 'ArrowDown')
        spd = add_constrain(spd, -10)
    update()
}

function handler_up() {
    dir = 'stop'
    update()
}

function add_constrain(val, inc){
    if (val + inc > 100)
        return 100
    if (val + inc < 0)
        return 0
    return val + inc
}

function update() {
    lbl_dir.innerHTML = 'direction: ' + dir
    lbl_spd.innerHTML = 'speed: ' + spd
    send('/upd/spd/' + spd);
    send('/upd/dir/' + dir);
}

function send(url) {
    let xhr = new XMLHttpRequest()
    xhr.open('GET', url)
    xhr.send()
}