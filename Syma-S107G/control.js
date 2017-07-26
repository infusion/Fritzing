
const clamp = (x, a, b) => Math.max(Math.min(x, b), a);

const SerialPort = require("serialport");
const Gamecontroller = require("gamecontroller");
const ctrl = new Gamecontroller("ps2");

ctrl.connect();

const serialPort = new SerialPort("/dev/cu.usbserial-A702YPTD", {
  baudrate: 9600,
  autoOpen: false
});

const state = [
  0xFF,
  0xAA,
  0, // Throttle
  63, // Yaw
  63, // Pitch
  63 // Trim
];

serialPort.open(function(err) {
  if (err) {
    console.log(err);
    return;
  }

  ctrl.on("data", function(data) {

    //data[2]: 0x80=center, 0xff rechts, 0x00=links
    //data[3]: 0x80center, 0x00 vor,
    
    state[2] = clamp(state[2] + (128 - data['axis:JOYL:Y']) / 100, 0, 127);
    state[3] = clamp(127 - data['axis:JOYR:X'] / 2, 0, 127);
    state[4] = clamp((data['axis:JOYR:Y'] - 2) / 2, 0, 127);

    serialPort.write(new Buffer(state));
  });
});

ctrl.on("error", function(x) {
  console.error(x)
});
