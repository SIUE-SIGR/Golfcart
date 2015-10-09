"""autogenerated by genpy from laser_sub/lasArray.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import laser_sub.msg

class lasArray(genpy.Message):
  _md5sum = "5bd8316bb420e1b5b48d1bf192e5bf85"
  _type = "laser_sub/lasArray"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """Num[] laserArray


================================================================================
MSG: laser_sub/Num
float32 centerX
float32 centerY
float32 radius

"""
  __slots__ = ['laserArray']
  _slot_types = ['laser_sub/Num[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       laserArray

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(lasArray, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.laserArray is None:
        self.laserArray = []
    else:
      self.laserArray = []

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      length = len(self.laserArray)
      buff.write(_struct_I.pack(length))
      for val1 in self.laserArray:
        _x = val1
        buff.write(_struct_3f.pack(_x.centerX, _x.centerY, _x.radius))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.laserArray is None:
        self.laserArray = None
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.laserArray = []
      for i in range(0, length):
        val1 = laser_sub.msg.Num()
        _x = val1
        start = end
        end += 12
        (_x.centerX, _x.centerY, _x.radius,) = _struct_3f.unpack(str[start:end])
        self.laserArray.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      length = len(self.laserArray)
      buff.write(_struct_I.pack(length))
      for val1 in self.laserArray:
        _x = val1
        buff.write(_struct_3f.pack(_x.centerX, _x.centerY, _x.radius))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.laserArray is None:
        self.laserArray = None
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.laserArray = []
      for i in range(0, length):
        val1 = laser_sub.msg.Num()
        _x = val1
        start = end
        end += 12
        (_x.centerX, _x.centerY, _x.radius,) = _struct_3f.unpack(str[start:end])
        self.laserArray.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_3f = struct.Struct("<3f")
