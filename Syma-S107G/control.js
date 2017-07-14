
function clamp(x, a, b) {
  return Math.max(Math.min(x, b), a);
}

let IS_OPEN = false;

const Gamecontroller = require("gamecontroller");
const ctrl = new Gamecontroller("ps2");

ctrl.connect();

const SerialPort = require("serialport");
const serialPort = new SerialPort("/dev/cu.usbserial-A702YPTD", {
  baudrate: 9600,
  autoOpen: false
});

serialPort.open(function(err) {
  if (err) {
    console.log(err);
    return;
  }
  IS_OPEN = true;
});

const state = [
  0xFF,
  0xAA,
  0, // Throttle
  63, // Yaw
  63, // Pitch
  63 // Trim
];

ctrl.on("data", function(data) {

  if (!IS_OPEN)
    return;

  //data[2]: 0x80=center, 0xff rechts, 0x00=links
  //data[3]: 0x80center, 0x00 vor,

  state[2] += (-data['axis:JOYL:Y'] + 0x80) / 100;
  state[3] = 63 + (-data['axis:JOYR:X'] + 0x80) / 2;
  state[4] = 63 + (data['axis:JOYR:Y'] - 0x80) / 2;

  state[2] = clamp(state[2], 0, 127);
  state[3] = clamp(state[3], 0, 127);
  state[4] = clamp(state[4], 0, 127);

  serialPort.write(new Buffer(state));
});

ctrl.on("error", function(x) {
  console.error(x)
});