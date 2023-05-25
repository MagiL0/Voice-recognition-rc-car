# 基于 ESP32 和 Blinker 的语音遥控小车
----
### 项目介绍
- 项目为咸鱼接单，抽了一天时间写的，没有特别按照格式，也不熟悉  Arduino 程序，因此可能代码风格一般。
### 项目环境
- 项目采用 1.8 版本的 **Arduino IDE** 编写，开发板为 `ESP32`，使用 `Blinker` 库进行开发，实现语音控制和无线控制的功能。
### Blinker 配置
- 项目使用了 `Blinker` 库，开发环境配置方法请参考官方文档。
- `Blinker` 中动作配置如下：
  ```json
  [
    {
        "cmd":{"voice":1},
        "text":"前进"
    },
    {
        "cmd":{"voice":2},
        "text":"后退"
    },
    {
        "cmd":{"voice":3},
        "text":"左转"
    },
    {
        "cmd":{"voice":4},
        "text":"右转"
    },
    {
        "cmd":{"voice":5},
        "text":"停止"
    }
  ]
  ```
- `Blinker` 中界面配置如下：
    ```
    {¨version¨¨2.0.0¨¨config¨{¨headerColor¨¨transparent¨¨headerStyle¨¨dark¨¨background¨{¨img¨¨assets/img/headerbg.jpg¨¨isFull¨«}}¨dashboard¨|{¨type¨¨ran¨¨t0¨¨滑动条¨¨clr¨¨#389BEE¨¨max¨¢47¨min¨¢-47¨bg¨É¨cols¨Ñ¨rows¨Ë¨key¨¨rpwm¨´x´É´y´Ñ}{ßCßDßEßFßGßHßIº0ßJº1ßKÉßLÑßMËßN¨lpwm¨´x´É´y´Ï}{ßCßDßE¨模式¨ßGßHßIÊßJÉßKÉßLÑßMËßN¨state¨´x´É´y´Í¨lstyle¨É}{ßCßDßE¨循迹速度¨ßGßHßI¢2kßJÉßKÉßLÑßMËßN¨speed¨´x´É´y´¤B}{ßCßDßE¨声控¨ßGßHßIÎßJÉßKÉßLÏßMÊßN¨voice¨´x´É´y´¤DßSÎ}{ßC¨deb¨¨mode¨ÉßKÉßLÑßMÌßN¨debug¨´x´É´y´É}÷¨actions¨|¦¨cmd¨¦ßW‡¨text¨‡Ê¨前进¨Ë¨后退¨Ì¨左转¨Í¨右转¨Î¨停止¨—÷¨triggers¨|{¨source¨¨voicecontrol¨¨source_zh¨ßVßR|´a´´b´´c´´d´÷¨state_zh¨|ßdßeßfßg÷}÷¨rt¨|÷}
    ```

----
- 有任何问题，请联系邮箱：`fengmagil@gmail.com`。
