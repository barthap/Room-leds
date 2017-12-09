/////////SERIAL PORT////////////////
var SerialPort = require("serialport");

console.log(process.argv);

var sp = new SerialPort("/dev/ttyS0", {
  baudRate: 115200,
  parser: SerialPort.parsers.readline('\r'),
  autoOpen: false,
});

sp.open(function(err) {
    if (err) {
        console.log("Port open error: ", err);
    }
    else {
        console.log("Port opened!");
    }
});

sp.on('data', function (data) {
  console.log('Serial data: ' + data);
});

///UDP SERVER
var PORT = 33333;
var HOST = '192.168.0.102';

var dgram = require('dgram');
var server = dgram.createSocket('udp4');

var colorsys = require('colorsys');

server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', function (message, remote) {
    console.log(remote.address + ':' + remote.port +' - ' + message);
	
	message += '';	//convert to string
	
	if(message.lastIndexOf("bed-rgb:") == 0)
	{
		let arr = message.split(':');
		
		let color = colorsys.rgb_to_hsv({ r: parseInt(arr[1]), g: parseInt(arr[2]), b: parseInt(arr[3]) });
		console.log(color);
		
		let hue = color[0].h*2.55/3.6;
		let sat = color[0].s*2.55;
		let val = color[0].v*2.55;
		
		let toSend = "B=" + parseInt(val) +"h" + parseInt(hue) + "s" + parseInt(sat);
		write(toSend);
	}
	else if(message.lastIndexOf("m:") == 0)
	{
		let arr = message.split(":");
		let toSend = "M=r"+arr[1]+"g"+arr[2]+"b"+arr[3];
		write(toSend);
	}
	else if(message.lastIndexOf("filter:") == 0)
	{
		let arr = message.split(":");
		write("Filter="+arr[1]);
	}

});

server.bind(PORT, HOST);


function write(message = null)
{
        if (sp.isOpen())  {
			if(message === null)
				message= process.argv[2];
			
			if(message === undefined) return;
			
			message += "\r";

			
            console.log("Writing serial data: " + message);
            sp.write(Buffer.from(message), function(err, res)
            {
                    if (err)
                    {
                            console.log(err);
                    }
					
            });
        }
        else {
            setTimeout(write, 50);
        }
}

setTimeout(write, 10);
