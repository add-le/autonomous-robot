

// port du serveur Web
var port_web = 3000

// dossier contenant les fichiers HTML que le serveur Web pourra envoyer au navigateur
var rep_html = "/home/spizzis/projet arduino/robot_l3/node/html"

// port com ou tty
var tty = "/dev/ttyACM0"
//var tty = "com11"

module.exports = {
    port_web: port_web,
    rep_html : rep_html,
    tty : tty,
}
