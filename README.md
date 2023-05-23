# Android Camera HAL 接口调试工具
本项目旨在记录 Android Camera Hardware Abstraction Layer (HAL) 接口的关键函数调用信息，便于进行深入的分析和调试。

## 功能
该项目提供了以下功能：
- 记录并写入 `initialize`、`configure_streams`、`process_capture_request` 和 `flush` 四个 Camera HAL 接口函数的调用信息。
- 记录并写入 `process_capture_result` 和 `notify` 两个callback函数的调用信息。

所有的记录信息将被保存在 `/data/vendor/camera/tlog/` 目录下，每个函数调用会生成一个文本文件。

## 安装
将 `dump.cpp` 中的内容，手动添加到当前 HAL 代码内调用。

## TODO
- **内联Hook实现** - 当前设计需要重新编译 Camera HAL 代码以插入我们的调试代码片段，这无疑增加了使用工具的复杂性。计划将调试代码片段以内联 hook 的形式插入，从而无需重新编译整个项目，使得此工具更加灵活和易用。
