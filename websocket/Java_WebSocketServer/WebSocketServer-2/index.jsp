<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>websocket web</title>
    <meta http-equiv="content-Type" content="text/html;charset=utf-8" />
    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="bootstrap/css/bootstrap.min.css" />
    <link rel="stylesheet" href="bootstrap/css/bootstrap-theme.min.css" />
    <!-- JQuery and Bootstrap JS -->
    <script src="JS/jquery.min.js"></script>
    <script src="bootstrap/js/bootstrap.min.js"></script>
    <!-- personal JS -->
    <script src="JS/myindex.js" charset="utf-8"></script>
  </head>
  <body>
    <div class="container">
        <div class="input-group" style="margin:10% auto;">
            <input type="text" id="msg" class="form-control" placeholder="input message" />
            <div class="input-group-btn btn-group-md">
                <button type="button" id="send" class="btn btn-default btn-md">发送</button>
            </div>
        </div>
        <div class="container">
            <div class="panel panel-default">
                <div class="panel-heading">信息记录</div>
                    <div class="panel-body">
                    </div>
            </div>
        </div>
    </div>
  </body>
</html>