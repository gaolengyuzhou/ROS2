#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

// 包含标准整数消息类型的头文件
#include <std_msgs/msg/int32.h> 

rclc_executor_t executor;

rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

rcl_publisher_t publisher; // 定义发布者
const char * topic_name = "count_topic";

int count = 0; // 定义计数变量
const rosidl_message_type_support_t * type_support =
  ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32);

void setup()
{
  Serial.begin(115200);
  // 设置通过串口进行MicroROS通信
  set_microros_serial_transports(Serial);
  // 延时时一段时间，等待设置完成
  delay(2000);
  // 初始化内存分配器
  allocator = rcl_get_default_allocator();
  // 创建初始化选项
  rclc_support_init(&support, 0, NULL, &allocator);
  // 创建节点 hello_microros
  rclc_node_init_default(&node, "hello_microros", "", &support);
  // 创建执行器
  rclc_executor_init(&executor, &support.context, 1, &allocator);

  // 新增：初始化发布者，发布的话题名为"count_topic"（可根据需求修改话题名）
  rclc_publisher_init_default(
    &publisher,
    &node,
    type_support,
    topic_name
  );
}

void loop()
{
  delay(100);
  // 循环处理数据
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));

  // Int32 message object
  std_msgs__msg__Int32 msg;
  // 填充计数到消息中
  msg.data = count;
  // 发布消息
  rcl_publish(&publisher, &msg, NULL);
  // 计数自增
  count++;
}
