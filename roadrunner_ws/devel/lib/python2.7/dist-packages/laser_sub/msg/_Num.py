"""autogenerated by genpy from laser_sub/Num.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class Num(genpy.Message):
  _md5sum = "65790d35a6f6d4aec392918077a52788"
  _type = "laser_sub/Num"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """float32 centerX
float32 centerY
float32 radius

"""
  __slots__ = ['centerX','centerY','radius']
  _slot_types = ['float32','float32','float32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       centerX,centerY,radius

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(Num, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.centerX is None:
        self.centerX = 0.
      if self.centerY is None:
        self.centerY = 0.
      if self.radius is None:
        self.radius = 0.
    else:
      self.centerX = 0.
      self.centerY = 0.
      self.radius = 0.

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
      _x = self
      buff.write(_struct_3f.pack(_x.centerX, _x.centerY, _x.radius))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      _x = self
      start = end
      end += 12
      (_x.centerX, _x.centerY, _x.radius,) = _struct_3f.unpack(str[start:end])
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
      _x = self
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
      end = 0
      _x = self
      start = end
      end += 12
      (_x.centerX, _x.centerY, _x.radius,) = _struct_3f.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_3f = struct.Struct("<3f")
