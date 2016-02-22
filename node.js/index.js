const Emitter = require('events').EventEmitter;
const util = require('util');

const PARSER_WAITING_DATA_LEN = 1;
const PARSER_WAITING_DATA = 2;

function Framer() {
  if (!(this instanceof Framer))
    return new Framer();

  Emitter.call(this);
  this._init();
}

util.inherits(Framer, Emitter);

Framer.prototype._init = function () {
  this.on('data', parse.bind(this));
  this._reset();
};

Framer.prototype._reset = function () {
  this._parser = {
    state: PARSER_WAITING_DATA_LEN,
    len: null, // UInt32LE
    lenArr: [],
    lenReaded: 0,
    dataArr: [],
    dataReaded: 0
  };
};

Framer.pack = function(data, len) {
  var lenBuf = new Buffer(4);
  var dataArr = [];

  if (typeof len === 'number') {
    // do nothing
  }
  else if (typeof data === 'string' || data instanceof Buffer)
    len = data.length;
  else if (data instanceof Array) {
    len = 0;
    data.forEach((buf) => len += buf.length);
  } else
    throw new Error(
                  '@param data must be an Buffer, String or Array of Buffers.');


  if (data instanceof Buffer)
    dataArr.push(data);
  else if (typeof data === 'string') {
    dataArr.push(new Buffer(data));
  }
  else if (data instanceof Array) {
    // validate array
    data.forEach((buf) => {
      if (!(buf instanceof Buffer))
        throw new Error('Elements or data array must be an Buffer');
    });
    dataArr = data;
  }

  lenBuf.writeUInt32LE(len, 0);
  dataArr.unshift(lenBuf);

  return Buffer.concat(dataArr, 4 + len);
};

Framer.unpack = function(data) {
  var bufLen;
  var dataLen;

  if (!(data instanceof Buffer))
    throw new Error('data must be an Buffer');

  bufLen = data.length;

  if (bufLen < 4)
    throw new Error('data must contain 4 or more bytes');

  dataLen = data.readUInt32LE(0);
  data = data.slice(4);

  if (dataLen !== data.length)
    throw new Error('Buffer is not contain all of data');

  return data;
};

/**
 * Parser implementation
 */

function parse(data) {
  var cursor;
  var needed;
  var toread;
  var buf;

  if (this._parser.state === PARSER_WAITING_DATA_LEN) {
    cursor = this._parser.lenReaded;
    needed = 4 - cursor;
    toread = Math.min(data.length, needed);

    if (toread < needed) {
      this._parser.lenArr.push(data);
      this._parser.lenReaded += data.length;
      return;
    }

    buf = data.slice(0, toread);
    data = data.slice(toread);

    if (buf.length !== 4)
      this._parser.lenArr.push(buf);

    if (this._parser.lenArr.length > 1)
      this._parser.len = Buffer.concat(this._parser.lenArr, 4).readUInt32LE(0);
    else
      this._parser.len = buf.readUInt32LE(0);
    this._parser.state = PARSER_WAITING_DATA;
  }

  // parser state is: PARSER_WAITING_DATA
  cursor = this._parser.dataReaded;
  needed = this._parser.len - cursor;
  toread = Math.min(data.length, needed);

  if (toread < needed) {
    this._parser.dataArr.push(data);
    this._parser.dataReaded += data.length;
    return;
  }

  buf = data.slice(0, toread);
  data = data.slice(toread);

  this._parser.dataArr.push(buf);

  if (this._parser.dataArr.length > 1)
    buf = Buffer.concat(this._parser.dataArr, this._parser.len);

  this.emit('frame', buf);

  // reset
  this._reset();

  // recursive call if you need
  if (data.length > 0)
    process.nextTick(parse.bind(this, data));
}

module.exports = Framer;
