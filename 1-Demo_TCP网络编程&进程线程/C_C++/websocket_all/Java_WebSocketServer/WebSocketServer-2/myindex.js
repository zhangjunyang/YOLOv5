$(function(){
    //创建WebSocket对象
    var url = "ws://localhost:8080/socketweb/server";
    var ws = "";
    $.fn.createws = function(){
        if('WebSocket' in window){
            ws = new WebSocket(url);
        }
        else if('MozWebSocket' in window){
            ws = new MozWebSocket(url);
        }
        ws.onopen = function(){
            console.log("已经打开了websocket连接，可以进行实时通信了");
        };
        ws.onmessage = function(e){
            console.log("接受到来自服务器端的数据:"+e.data);
            if(e.data == "close"){
                //这里要注意onclose的第一个参数必须是1000或者3000—4999
                ws.onclose(1000,"服务器通知关闭");
            }
            else{
                $(".panel-body").append("<p>"+e.data"+"</p>");
            }
        }；
        ws.onerror = function(e){
            console.log("websocket连接错误"+e.error);
            ws.close(3000,"websocket连接异常导致的关闭");
        };
        ws.onclose = function(e){
            console.log("websocket关闭连接:"+e.error);
        };
    };
    $.fn.createws();
    /**
     *为按钮添加发送信息事件
    **/
    $("#send").click(function(){
        ws.send($("#msg").val());
        $("#msg").val("");
    });
});