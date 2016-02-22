import test from 'ava';
import Framer from './';

test('pack', t => {
  var packed = Framer.pack('test');
  check();

  packed = Framer.pack(new Buffer('test'));
  check();

  packed = Framer.pack([new Buffer('te'), new Buffer('st')]);
  check();

  function check() {
    t.true(packed instanceof Buffer);
    t.is(packed.length, 8);
  }
});

test('unpack', t => {
  var packed = Framer.pack('test');
  var unpacked = Framer.unpack(packed);

  t.true(unpacked instanceof Buffer);
  t.is(unpacked.length, 4);
  t.is(unpacked.toString(), 'test');
});

test('parse', t => {
  var framer = new Framer();
  
  framer.on('frame', function (frame) {
    t.true(frame instanceof Buffer);
    t.is(frame.length, 4);
    t.is(frame.toString(), 'test');
  });

  framer.emit('data', Framer.pack('test'));
});
