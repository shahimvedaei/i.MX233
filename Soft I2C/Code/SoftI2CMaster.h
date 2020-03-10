  // 'initialized' will be:
  //    255 on startup,
  //    0 if beginTransmission() was called and successful,
  //    any other value if there was an error during beginTransmission().
  char initialized;

  void i2c_writebit( char c );
  char i2c_readbit(void);
  void i2c_init(void);
  void i2c_start(void);
  void i2c_repstart(void);
  void i2c_stop(void);
  char i2c_write( char c );
  char i2c_read( char ack );
  
  char beginTransmission(char address);
  char endTransmission(void);
  void my_write(char*, char);
  char requestFrom(char address);
  char my_read();
  char readLast();
