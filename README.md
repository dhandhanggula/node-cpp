<!-- Title -->
<span align = "center">

# Dhandhanggula Node CPP
  <b>Implementation of Dhandhanggula node written in Arduino</b>

Development

</span>
<!-- End of Title -->

## About this project
Aim to build lora physical wireless mesh network which can used as a public network based on LoRa modulation. This repository is contain main program of node using Arduino.

## Library(s) used
- LoRa v0.8.0 by sandeepmistry https://github.com/sandeepmistry/arduino-LoRa
- RTClib v2.0.2 by Adafruit

## Message structure
Message structure is same in any condition. The sturcture is shown below.

```
Message Code | Message ID | Node ID | Destination ID | Route Path | Payload
```

## Message Code
| Code | Desrciption |
|:---:|---|
|`00`| Ping |
|`01`| Answer ping |
|`10`| General message |
|`20`| Route Request (RREQ) |
|`21`| Route Repply (RREP) |
|`30`| Route Error (RERR) |

## Response Code
| Response | Description |
|:---:|---|
|`200`| Success sending message with no error. |
|`201`| Route path is created. This code is get after route discovery process and node get RREP from destination node.|
|`204`| No payload content. Payload must not blank. |
|`400`| Bad request. Syntax is error. |
|`404`| Destination in RREQ not found. |
|`408`| Request Timeout. |
|`411`| Header content is not fulfill the requirement. |
|`413`| Payload too large. Maximum payload is limited to 150 character. |
|`414`| Route path too long. Maximum relay node is 6 node. |
