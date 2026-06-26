CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

# 原本的 signal_system
SYSTEM_OBJ = build/main.o \
             build/signal_generator.o \
             build/signal_processor.o \
             build/consumer.o \
             build/producer.o \
			 build/thread_pool.o

# TCP Sender
SENDER_OBJ = build/signal_sender.o \
             build/signal_generator.o

# TCP Receiver
RECEIVER_OBJ = build/signal_receiver.o \
               build/signal_processor.o

TARGET_SYSTEM = signal_system
TARGET_SENDER = signal_sender
TARGET_RECEIVER = signal_receiver

all: $(TARGET_SYSTEM) $(TARGET_SENDER) $(TARGET_RECEIVER)

$(TARGET_SYSTEM): $(SYSTEM_OBJ)
	$(CXX) $^ -o $@

$(TARGET_SENDER): $(SENDER_OBJ)
	$(CXX) $^ -o $@

$(TARGET_RECEIVER): $(RECEIVER_OBJ)
	$(CXX) $^ -o $@

build/%.o: src/%.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f build/*.o \
	      $(TARGET_SYSTEM) \
	      $(TARGET_SENDER) \
	      $(TARGET_RECEIVER)