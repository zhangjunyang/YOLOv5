<br>
using System;<br>
using System.Net;<br>
using System.Net.Sockets;<br>
using System.Security.Cryptography;<br>
using System.Text;<br>
using System.Text.RegularExpressions;<br>
 <br>
namespace WebSocket<br>
{<br>
class Program<br>
{<br>
static void Main(string[] args)<br>
{<br>
int port = 1818;<br>
byte[] buffer = new byte[1024];<br>
 <br>
IPEndPoint localEP = new IPEndPoint(IPAddress.Any, port);<br>
Socket listener = new Socket(localEP.Address.AddressFamily,SocketType.Stream, ProtocolType.Tcp);<br>
try{<br>
listener.Bind(localEP);<br>
listener.Listen(10);<br>
 <br>
Console.WriteLine("等待客户端连接....");<br>
Socket sc = listener.Accept();//接受一个连接<br>
Console.WriteLine("接受到了客户端："+sc.RemoteEndPoint.ToString()+"连接....");<br>
//握手<br>
int length = sc.Receive(buffer);//接受客户端握手信息<br>
sc.Send(PackHandShakeData(GetSecKeyAccetp(buffer,length)));<br>
Console.WriteLine("已经发送握手协议了....");<br>
//接受客户端数据<br>
Console.WriteLine("等待客户端数据....");<br>
length = sc.Receive(buffer);//接受客户端信息<br>
string clientMsg=AnalyticData(buffer, length);<br>
Console.WriteLine("接受到客户端数据：" + clientMsg);<br>
 <br>
//发送数据<br>
string sendMsg = "您好，" + clientMsg;<br>
Console.WriteLine("发送数据：“"+sendMsg+"” 至客户端....");<br>
sc.Send(PackData(sendMsg));<br>
Console.WriteLine("演示Over!");<br>
 <br>
}<br>
catch (Exception e)<br>
{<br>
Console.WriteLine(e.ToString());<br>
}<br>
}<br>
 <br>
/// <summary><br>
/// 打包握手信息<br>
/// </summary><br>
/// <param name="secKeyAccept">Sec-WebSocket-Accept<br>
/// <returns>数据包</returns><br>
private static byte[] PackHandShakeData(string secKeyAccept)<br>
{<br>
var responseBuilder = new StringBuilder();<br>
responseBuilder.Append("HTTP/1.1 101 Switching Protocols" + Environment.NewLine);<br>
responseBuilder.Append("Upgrade: websocket" + Environment.NewLine);<br>
responseBuilder.Append("Connection: Upgrade" + Environment.NewLine);<br>
responseBuilder.Append("Sec-WebSocket-Accept: " + secKeyAccept + Environment.NewLine + Environment.NewLine);<br>
//如果把上一行换成下面两行，才是thewebsocketprotocol-17协议，但居然握手不成功，目前仍没弄明白！<br>
//responseBuilder.Append("Sec-WebSocket-Accept: " + secKeyAccept + Environment.NewLine);<br>
//responseBuilder.Append("Sec-WebSocket-Protocol: chat" + Environment.NewLine);<br>
 <br>
return Encoding.UTF8.GetBytes(responseBuilder.ToString());<br>
}<br>
 <br>
/// <summary><br>
/// 生成Sec-WebSocket-Accept<br>
/// </summary><br>
/// <param name="handShakeText">客户端握手信息<br>
/// <returns>Sec-WebSocket-Accept</returns><br>
private static string GetSecKeyAccetp(byte[] handShakeBytes,int bytesLength)<br>
{<br>
string handShakeText = Encoding.UTF8.GetString(handShakeBytes, 0, bytesLength);<br>
string key = string.Empty;<br>
Regex r = new Regex(@"Sec-WebSocket-Key:(.*?)rn");<br>
Match m = r.Match(handShakeText);<br>
if (m.Groups.Count != 0)<br>
{<br>
key = Regex.Replace(m.Value, @"Sec-WebSocket-Key:(.*?)rn", "$1").Trim();<br>
}<br>
byte[] encryptionString = SHA1.Create().ComputeHash(Encoding.ASCII.GetBytes(key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"));<br>
return Convert.ToBase64String(encryptionString);<br>
}<br>
 <br>
/// <summary><br>
/// 解析客户端数据包<br>
/// </summary><br>
/// <param name="recBytes">服务器接收的数据包<br>
/// <param name="recByteLength">有效数据长度<br>
/// <returns></returns><br>
private static string AnalyticData(byte[] recBytes, int recByteLength)<br>
{<br>
if (recByteLength < 2) { return string.Empty; }<br>
 <br>
bool fin = (recBytes[0] & 0x80) == 0x80; // 1bit，1表示最后一帧<br>
if (!fin){<br>
return string.Empty;// 超过一帧暂不处理<br>
}<br>
 <br>
bool mask_flag = (recBytes[1] & 0x80) == 0x80; // 是否包含掩码<br>
if (!mask_flag){<br>
return string.Empty;// 不包含掩码的暂不处理<br>
}<br>
 <br>
int payload_len = recBytes[1] & 0x7F; // 数据长度<br>
 <br>
byte[] masks = new byte[4];<br>
byte[] payload_data;<br>
 <br>
if (payload_len == 126){<br>
Array.Copy(recBytes, 4, masks, 0, 4);<br>
payload_len = (UInt16)(recBytes[2] << 8 | recBytes[3]);<br>
payload_data = new byte[payload_len];<br>
Array.Copy(recBytes, 8, payload_data, 0, payload_len);<br>
 <br>
}else if (payload_len == 127){<br>
Array.Copy(recBytes, 10, masks, 0, 4);<br>
byte[] uInt64Bytes = new byte[8];<br>
for (int i = 0; i < 8; i++){<br>
uInt64Bytes[i] = recBytes[9 - i];<br>
}<br>
UInt64 len = BitConverter.ToUInt64(uInt64Bytes, 0);<br>
 <br>
payload_data = new byte[len];<br>
for (UInt64 i = 0; i < len; i++){<br>
payload_data[i] = recBytes[i + 14];<br>
}<br>
}else{<br>
Array.Copy(recBytes, 2, masks, 0, 4);<br>
payload_data = new byte[payload_len];<br>
Array.Copy(recBytes, 6, payload_data, 0, payload_len);<br>
 <br>
}<br>
 <br>
for (var i = 0; i < payload_len; i++){<br>
payload_data[i] = (byte)(payload_data[i] ^ masks[i % 4]);<br>
}<br>
return Encoding.UTF8.GetString(payload_data);<br>
}<br>
 <br>
 <br>
/// <summary><br>
/// 打包服务器数据<br>
/// </summary><br>
/// <param name="message">数据<br>
/// <returns>数据包</returns><br>
private static byte[] PackData(string message)<br>
{<br>
byte[] contentBytes = null;<br>
byte[] temp = Encoding.UTF8.GetBytes(message);<br>
 <br>
if (temp.Length < 126){<br>
contentBytes = new byte[temp.Length + 2];<br>
contentBytes[0] = 0x81;<br>
contentBytes[1] = (byte)temp.Length;<br>
Array.Copy(temp, 0, contentBytes, 2, temp.Length);<br>
}else if (temp.Length < 0xFFFF){<br>
contentBytes = new byte[temp.Length + 4];<br>
contentBytes[0] = 0x81;<br>
contentBytes[1] = 126;<br>
contentBytes[2] = (byte)(temp.Length & 0xFF);<br>
contentBytes[3] = (byte)(temp.Length >> 8 & 0xFF);<br>
Array.Copy(temp, 0, contentBytes, 4, temp.Length);<br>
}else{<br>
// 暂不处理超长内容<br>
}<br>
 <br>
return contentBytes;<br>
}<br>
}<br>
}<br>

