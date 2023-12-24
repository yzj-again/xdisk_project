TARGET=libxcom.so
OBJS=xserver_task.o xthread.o xthread_pool.o
CPPFLAGS= -fPIC #±‡“Î‘¥¬Î
LIBS=-levent -lpthread
$(TARGET):$(OBJS)
    g++ -shared $^ -o $@ $(TARGET)
clean:
    rm -rf $(OBJS) $(TARGET)