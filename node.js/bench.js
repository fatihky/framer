const Framer = require('./');

const framer = new Framer();

const COUNT = 1000000;
var start = Date.now();
var parsed = 0;

framer.on('frame', function (frame) {
  parsed++;

  if (parsed === COUNT) {
    var ms = Date.now() - start;
    var thr = (COUNT * 1000) / ms;
    console.log('Decode throughput: %s frames per second.',
                                                          thr.toLocaleString());
  }
});

// encode
var bufArr = new Array(COUNT);

for (var i = 0; i < COUNT; i++) {
  bufArr[i] = Framer.pack('test', 4);
}

// encode bencmark
{
  var ms = Date.now() - start;
  var thr = (COUNT * 1000) / ms;
  console.log('Encode throughput: %s frames per second.', thr.toLocaleString());
}

// start decode benchmark
{
  var data = Buffer.concat(bufArr, COUNT * 8);
  start = Date.now();
  framer.emit('data', data);
}
