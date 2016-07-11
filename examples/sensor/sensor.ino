  String msg = String( reinterpret_cast< char const* >(payload) ) ;
  String top = String(topic);

  iotpipe.update_outputs(top, msg);
