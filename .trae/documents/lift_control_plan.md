# 第五个电机控制方案 - 实现计划

## 方案评估

### 方案A：扩展单关节点动模式
- **优点**：修改量小，利用现有架构
- **缺点**：
  - 单关节点动模式是速度控制，不适合位置控制（转动圈数）
  - 需要修改多个现有函数
  - 功能混合，不够清晰

### 方案B：新建升降平台控制模式
- **优点**：
  - 功能独立，逻辑清晰
  - 专门为位置控制设计
  - 不影响现有功能
  - 扩展性好
- **缺点**：需要新增模式和相关函数

## 推荐方案：方案B - 新建升降平台控制模式

## 实施步骤

### [ ] 任务1：扩展Winch结构体以支持第5个电机
- **Priority**：P0
- **Depends On**：None
- **Description**：
  - 在Winch结构体中添加第5个电机的相关字段
  - 包括电机对象、PID控制器、参考位置等
- **Success Criteria**：
  - Winch结构体能够存储第5个电机的所有必要信息
- **Test Requirements**：
  - `programmatic` TR-1.1：编译通过，无语法错误
  - `human-judgement` TR-1.2：结构体定义清晰，字段命名规范

### [ ] 任务2：修改Winch_Create和Winch_SetPID函数
- **Priority**：P0
- **Depends On**：任务1
- **Description**：
  - 修改Winch_Create函数，为第5个电机分配内存
  - 修改Winch_SetPID函数，为第5个电机设置PID参数
- **Success Criteria**：
  - 第5个电机能够正确初始化并设置PID参数
- **Test Requirements**：
  - `programmatic` TR-2.1：编译通过，无语法错误
  - `programmatic` TR-2.2：运行时无内存分配错误

### [ ] 任务3：添加升降平台控制模式
- **Priority**：P1
- **Depends On**：任务1, 任务2
- **Description**：
  - 在Winch_mode枚举中添加Winch_mode_Lift模式
  - 添加Winch_LiftControl函数处理升降平台控制逻辑
  - 在Winch_Calc_Output中添加对应case
- **Success Criteria**：
  - 新的升降平台模式能够被正确识别和处理
- **Test Requirements**：
  - `programmatic` TR-3.1：编译通过，无语法错误
  - `programmatic` TR-3.2：模式切换正常

### [ ] 任务4：扩展上位机命令处理
- **Priority**：P1
- **Depends On**：任务3
- **Description**：
  - 在Winch_RxCallback中添加升降平台控制命令
  - 支持设置转动方向和转动圈数
- **Success Criteria**：
  - 上位机能够发送命令控制升降平台
- **Test Requirements**：
  - `programmatic` TR-4.1：编译通过，无语法错误
  - `human-judgement` TR-4.2：命令格式清晰，易于使用

### [ ] 任务5：修改Winch_Send_Current函数
- **Priority**：P0
- **Depends On**：任务1
- **Description**：
  - 修改Winch_Send_Current函数，发送第5个电机的电流
  - 使用BSP_CAN_Transmit_58函数发送
- **Success Criteria**：
  - 第5个电机能够接收到电流命令
- **Test Requirements**：
  - `programmatic` TR-5.1：编译通过，无语法错误
  - `programmatic` TR-5.2：电机能够正常转动

### [ ] 任务6：实现升降平台控制逻辑
- **Priority**：P1
- **Depends On**：任务3, 任务5
- **Description**：
  - 实现Winch_LiftControl函数
  - 支持位置控制（转动圈数）
  - 支持方向控制
- **Success Criteria**：
  - 升降平台能够按照指定方向和圈数转动
- **Test Requirements**：
  - `programmatic` TR-6.1：编译通过，无语法错误
  - `human-judgement` TR-6.2：控制精度满足要求

### [ ] 任务7：测试和调试
- **Priority**：P2
- **Depends On**：任务1-6
- **Description**：
  - 测试所有功能是否正常
  - 调试PID参数
  - 验证控制精度
- **Success Criteria**：
  - 升降平台能够稳定、准确地控制
- **Test Requirements**：
  - `programmatic` TR-7.1：无运行时错误
  - `human-judgement` TR-7.2：控制效果良好

## 技术实现细节

### 数据结构扩展
- 在Winch结构体中添加：
  - `CAN_Motor *CAN1_M2006[5];`（扩展为5个电机）
  - `PID *M2006_speed_pid[5];`
  - `PID *M2006_position_pid[5];`
  - `float lift_ref_pos;`（升降平台目标位置）
  - `float lift_current_pos;`（升降平台当前位置）
  - `uint8_t lift_direction;`（升降方向）

### 控制逻辑
- **位置控制**：使用位置环+速度环的双环控制
- **圈数计算**：1圈 = 36 * 8192 / 360 编码器 ticks
- **方向控制**：正方向为上升，负方向为下降

### 上位机命令格式
- 启动升降平台模式：`"LMode"`
- 设置目标位置：`"#Lift pos:{圈数} dir:{方向}"`
  - 方向：0=上升，1=下降

### 安全考虑
- 添加位置限位检查
- 添加电流保护
- 添加超时保护

## 预期效果
- 能够通过上位机命令控制升降平台的升降
- 控制精度达到±0.1圈
- 运行稳定，无抖动
- 不影响现有4个电机的功能