#!/usr/bin/env python
# license removed for brevity
import rospy, time
from std_msgs.msg import String

def talker():
    pub = rospy.Publisher('psoc_tx', String)#, queue_size=1000)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    msg = "B 95\r"
    while not rospy.is_shutdown():
        pub.publish(msg)
        rospy.loginfo("%s", msg);
        msg = ""
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
