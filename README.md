# _FreeRTOS Queues_

FreeRTOS Queues are a thread-safe component for passing messages between tasks.
See more [here](https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32/api-reference/system/freertos.html#queue-api) (you may need to change the ESP-IDF version).

### Includes

    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "freertos/queue.h"

### Creating a Queue

The constructor accepts the number of items to be stored and the size of each item.
The items can be `structs` or `arrays` or `pointers` and they are stored by value - not reference.

Here the queue will store array of int16s, and it will only keep a single item (this is to test how different enqueuing mechanisms will behave):

    int16_t testy_value[4] = {0, 0, 0, 0};
    testy_queue = xQueueCreate(1, sizeof(testy_value));

### Enqueuing Items

There are 3 options:

- `xQueueSendToFront`
- `xQueueSendToBack`
- `xQueueOverwrite`

The first 2 will only add items to the queue if it's not full - if it's full (i.e. it already contains the number of elements that it was initialized with, the items will be dropped).

    xQueueSendToBack(testy_queue, (void *) testy_value, 0);

The 3rd option only works with queue of size 1 and if it's empty, the item will be added, if it's full, the existing item will be overwritten.
This option makes sense if you have a high-speed sensor pushing a lot of values, but you only care about the latest one.

    xQueueOverwrite(testy_queue, (void *) testy_value);

### Receiving Items

When the items are received, they are removed from the queue. It's done by `xQueueReceive` and it will return `pdTRUE` on success or `pdFALSE` if it's empty.

    xQueueReceive(testy_queue, (void *) read_value, 0);

### Scenarios
To try the different scenarios, update the sending method in the writing task and update the task delays for each of the task.

#### Writing Faster than Reading (with send)

In this case the writer uses `xQueueSendToBack` and writes 4-times faster than the reader reads the values:

    I (10) Queue-Test: Starting the writing task...
    I (10) Queue-Test: Sent 1
    I (260) Queue-Test: Starting the reading task...
    I (260) Queue-Test: Received 1
    I (1307) Queue-Test: Sent 2
    I (1807) Queue-Test: Sent 3
    I (2307) Queue-Test: Sent 4
    I (2807) Queue-Test: Sent 5
    I (3057) Queue-Test: Received 2
    I (3307) Queue-Test: Sent 6
    I (3807) Queue-Test: Sent 7
    I (4307) Queue-Test: Sent 8
    I (4807) Queue-Test: Sent 9
    I (5057) Queue-Test: Received 6
    I (5307) Queue-Test: Sent 10
    I (5807) Queue-Test: Sent 11
    I (6307) Queue-Test: Sent 12
    I (6807) Queue-Test: Sent 13
    I (7057) Queue-Test: Received 10
    I (7307) Queue-Test: Sent 14
    I (7807) Queue-Test: Sent 15
    I (8307) Queue-Test: Sent 16
    I (8807) Queue-Test: Sent 17
    I (9057) Queue-Test: Received 14
    I (9307) Queue-Test: Sent 18
    I (9807) Queue-Test: Sent 19
    I (10307) Queue-Test: Sent 20

What happens is that once the queue is full (i.e. after receiving 1 item), all the following items are dropped until the queue is read and emptied.

#### Writing Faster than Reading (with overwrite)

In this case the writer uses `xQueueOverwrite` and still writes 4-times faster than the reader reads the values:

    I (10) Queue-Test: Starting the writing task...
    I (10) Queue-Test: Sent 1
    I (260) Queue-Test: Starting the reading task...
    I (260) Queue-Test: Received 1
    I (1307) Queue-Test: Sent 2
    I (1807) Queue-Test: Sent 3
    I (2307) Queue-Test: Sent 4
    I (2807) Queue-Test: Sent 5
    I (3057) Queue-Test: Received 5
    I (3307) Queue-Test: Sent 6
    I (3807) Queue-Test: Sent 7
    I (4307) Queue-Test: Sent 8
    I (4807) Queue-Test: Sent 9
    I (5057) Queue-Test: Received 9
    I (5307) Queue-Test: Sent 10
    I (5807) Queue-Test: Sent 11
    I (6307) Queue-Test: Sent 12
    I (6807) Queue-Test: Sent 13
    I (7057) Queue-Test: Received 13
    I (7307) Queue-Test: Sent 14
    I (7807) Queue-Test: Sent 15
    I (8307) Queue-Test: Sent 16
    I (8807) Queue-Test: Sent 17
    I (9057) Queue-Test: Received 17
    I (9307) Queue-Test: Sent 18

Now, when the queue is full, the new item overwrites the old item and the reader always receives the latest item.

#### Reading Faster than Writing

In this case it doesn't matter which method is used by the writer as the queue is always empty because the reader reads 4-times faster:

    I (10) Queue-Test: Starting the writing task...
    I (10) Queue-Test: Sent 1
    I (260) Queue-Test: Starting the reading task...
    I (260) Queue-Test: Received 1
    I (1807) Queue-Test: No data received...
    I (2307) Queue-Test: No data received...
    I (2807) Queue-Test: No data received...
    I (3057) Queue-Test: Sent 2
    I (3307) Queue-Test: Received 2
    I (3807) Queue-Test: No data received...
    I (4307) Queue-Test: No data received...
    I (4807) Queue-Test: No data received...
    I (5057) Queue-Test: Sent 3
    I (5307) Queue-Test: Received 3
    I (5807) Queue-Test: No data received...
    I (6307) Queue-Test: No data received...
    I (6807) Queue-Test: No data received...
    I (7057) Queue-Test: Sent 4
    I (7307) Queue-Test: Received 4
    I (7807) Queue-Test: No data received...
