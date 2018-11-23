<script type="text/javascript">
  var ws = new WebSocket('ws://localhost:8080');
  ws.onopen = function () {
    console.log('client: ws connection is open');
    ws.send('上海市人民广场');
  };
  ws.onmessage = function (e) {
    console.log('client: received %s', e.data);
  };
  ws.onclose = function () {
    console.log('client: ws connection is closed');

  };
</script>
-------------------------------------------------------------------------------
<script src="JS/jquery.min.js"></script>
<script>
    $(function(){
        //websocket测试服务器
        var url = "ws://localhost:8080";
        var ws = null;
        function createws(){
            if('WebSocket' in window)
                ws = new WebSocket(url);
            else if('MOzWebSocket' in window)
                ws = new MozWebSocket(url);
            else
                console.log("浏览器太旧，不支持");
        }
        function btnfun(){
            createws();
            //成功建立WebSocket连接时触发onopen事件，通常客户端发送数据都是放在open事件里面
            ws.onopen = function(){
                console.log("connected");
                ws.send("hello world");
            };
            //接受服务器响应数据时触发onmessage事件
            ws.onmessage = function(event){
                console.log("receive message:"+event.data);
                //关闭websocket连接
                ws.close(999,"close normal");
            };
            //服务器处理异常，通常在服务器里try-catch发生异常时或者连接异常时触发onerror事件
            ws.onerror = function(err){
                console.log("error: "+err);
                ws.close(1000,"close after error");
            };
            //websocket连接关闭时触发onclose事件
            ws.onclose = function(event){
                console.log("close reason: "+event.reason);
            };
        };
        //为button绑定事件
        $("#test").click($.fn.btnfun);
    });
</script>
------------------------------------------------------------------------------
<script>
// Initialize WebSocket connection and event handlers
function setup() {
    output = document.getElementById("output");
    ws = new WebSocket("ws://localhost:8080");
    // Listen for the connection open event then call the sendMessage function
    ws.onopen = function(e) {
        log("Connected");
        sendMessage("这是发送的数据")
    }
    // Listen for the close connection event
    ws.onclose = function(e) {
        log("Disconnected: " + e.reason);
    }
    // Listen for connection errors
    ws.onerror = function(e) {
        log("Error ");
    }
    // Listen for new messages arriving at the client
    ws.onmessage = function(e) {
        log("Message received: " + e.data);
    // Close the socket once one message has arrived.
        ws.close();
    }
}
// Send a message on the WebSocket.
function sendMessage(msg){
    ws.send(msg);
    log("Message sent");
}
// Display logging information in the document.
function log(s) {
    var p = document.createElement("p");
    p.style.wordWrap = "break-word";
    p.textContent = s;
    output.appendChild(p);
    // Also log information on the javascript console
    console.log(s);
}
// Start running the example.
setup();
</script>
-----------------------------------------------------------------------------
<script>
(function(){
    var $ = function(id){return document.getElementById(id) || null;}
    var wsServer = 'ws://127.0.0.1:8080'; 
    var ws = new WebSocket(wsServer);
    var isConnect = false;
    ws.onopen = function (evt) { onOpen(evt) }; 
    ws.onclose = function (evt) { onClose(evt) }; 
    ws.onmessage = function (evt) { onMessage(evt) }; 
    ws.onerror = function (evt) { onError(evt) }; 
    function onOpen(evt) { 
        console.log("连接服务器成功");
        isConnect = true;
    } 
    function onClose(evt) { 
        //console.log("Disconnected"); 
    } 
    function onMessage(evt) {
        console.log('Retrieved data from server: ' + evt.data);
    }
    function onError(evt) { 
        //console.log('Error occured: ' + evt.data); 
    }
    function sendMsg() {
        if(isConnect){
            ws.send('hello');
        }
    }
})();
</script>
-----------------------------------------------------------------------------------------
<script type="text/javascript">  
    //var webSocket =  new WebSocket('ws://localhost:8080/testWebSocketProtocol/websocket');
    var webSocket =  new WebSocket('ws://localhost:8080');

    webSocket.onerror = function(event) {  
        onError(event)  
    };  

    webSocket.onopen = function(event) {  
        onOpen(event)  
    };  

    webSocket.onmessage = function(event) {  
        onMessage(event)  
    };  

    function onMessage(event) {  
        document.getElementById('messages').innerHTML   
            += '<br />Get Message From Server: ' + event.data;  
    }  

    function onOpen(event) {  
        document.getElementById('messages').innerHTML   
            = 'Connection established';  
    }  

    function onError(event) {  
        alert(event.data);  
        alert("error");  
    }  

    function start() {  
        webSocket.send('hello');  
        return false;  
    }  
</script>
-----------------------------------------------------------------------------------------
