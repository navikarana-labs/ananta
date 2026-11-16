/*
 * Copyright 2026 Lokesh Kumar N. (Navikarana Labs)
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/kernel.h>

CAN_MSGQ_DEFINE(can_rx_queue, 4);

int main(void) {

  const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(fdcan1));

  bool can_rdy = device_is_ready(dev);
  if (!can_rdy) {
    printk("CAN not ready\nAbort...\n");
    return -1;
  }

  can_mode_t capabilities = 0;
  int ret = can_get_capabilities(dev, &capabilities);

  if (ret != 0) {
    printk("Cannot retrieve capabilities!\nAbort...\n");
    return -1;
  }

  bool loopback_support = ((capabilities & CAN_MODE_LOOPBACK) != 0);
  if (!loopback_support) {
    printk("Loopback Not Supported!\nAbort...\n");
    return -1;
  }

  ret = can_set_mode(dev, CAN_MODE_LOOPBACK);
  if (ret != 0) {
    printk("Failed to set loopback mode!\nError: %d\nAbort...\n", ret);
    return -1;
  }

  const struct can_filter filter = {
      .id = 0x123,
      .mask = CAN_STD_ID_MASK,
      .flags = 0,
  };

  int filter_id = can_add_rx_filter_msgq(dev, &can_rx_queue, &filter);
  if (filter_id < 0) {
    printk("Failed to add RX filter: %d\n", filter_id);
    return -1;
  }

  ret = can_start(dev);
  if (ret != 0) {
    printk("Failed to start CAN controller!\nError: %d\nAbort...\n", ret);
    return -1;
  }

  const struct can_frame frame = {
      .id = 0x123, .dlc = 2, .flags = 0, .data = {0xAB, 0xCD}};

  int send = can_send(dev, &frame, K_MSEC(100), NULL, NULL);

  printk("response from can_send() = %d\n", send);

  static struct can_frame frame_rec = {
      .id = 0x123, .dlc = 2, .flags = 0, .data = {0}};

  int rec = k_msgq_get(&can_rx_queue, &frame_rec, K_MSEC(100));

  if (rec < 0) {
    printk("Failed to receive CAN frame: %d\n", rec);
    return -1;
  } else {
    printk("Received CAN frame: id=%#x, dlc=%d, data={%x, %x}\n", frame_rec.id,
           frame_rec.dlc, frame_rec.data[0], frame_rec.data[1]);
  }

  while (1) {
    printk("Project Ananta - Motion Controller\n"
           "CAN device ready\n"
           "Loopback mode configured\n"
           "RX filter registered: %d\n"
           "CAN controller started\n\n",
           filter_id);
    k_sleep(K_SECONDS(1));
  }
}
