<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form 覆寫 Dispose 以清除元件清單。
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing AndAlso components IsNot Nothing Then
            components.Dispose()
        End If
        MyBase.Dispose(disposing)
    End Sub

    '為 Windows Form 設計工具的必要項
    Private components As System.ComponentModel.IContainer

    '注意: 以下為 Windows Form 設計工具所需的程序
    '可以使用 Windows Form 設計工具進行修改。
    '請不要使用程式碼編輯器進行修改。
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.button2 = New System.Windows.Forms.Button
        Me.label2 = New System.Windows.Forms.Label
        Me.label1 = New System.Windows.Forms.Label
        Me.textBox2 = New System.Windows.Forms.TextBox
        Me.textBox1 = New System.Windows.Forms.TextBox
        Me.button1 = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'button2
        '
        Me.button2.Location = New System.Drawing.Point(167, 82)
        Me.button2.Name = "button2"
        Me.button2.Size = New System.Drawing.Size(100, 23)
        Me.button2.TabIndex = 17
        Me.button2.Text = "Read From ＤＩ"
        Me.button2.UseVisualStyleBackColor = True
        '
        'label2
        '
        Me.label2.AutoSize = True
        Me.label2.Location = New System.Drawing.Point(165, 23)
        Me.label2.Name = "label2"
        Me.label2.Size = New System.Drawing.Size(20, 12)
        Me.label2.TabIndex = 16
        Me.label2.Text = "DI:"
        '
        'label1
        '
        Me.label1.AutoSize = True
        Me.label1.Location = New System.Drawing.Point(21, 23)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(24, 12)
        Me.label1.TabIndex = 15
        Me.label1.Text = "DO:"
        '
        'textBox2
        '
        Me.textBox2.Location = New System.Drawing.Point(167, 38)
        Me.textBox2.Name = "textBox2"
        Me.textBox2.Size = New System.Drawing.Size(100, 22)
        Me.textBox2.TabIndex = 14
        Me.textBox2.TextAlign = System.Windows.Forms.HorizontalAlignment.Right
        '
        'textBox1
        '
        Me.textBox1.Location = New System.Drawing.Point(21, 38)
        Me.textBox1.Name = "textBox1"
        Me.textBox1.Size = New System.Drawing.Size(100, 22)
        Me.textBox1.TabIndex = 13
        Me.textBox1.TextAlign = System.Windows.Forms.HorizontalAlignment.Right
        '
        'button1
        '
        Me.button1.Location = New System.Drawing.Point(23, 82)
        Me.button1.Name = "button1"
        Me.button1.Size = New System.Drawing.Size(98, 23)
        Me.button1.TabIndex = 12
        Me.button1.Text = "Write To ＤＯ"
        Me.button1.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(292, 142)
        Me.Controls.Add(Me.button2)
        Me.Controls.Add(Me.label2)
        Me.Controls.Add(Me.label1)
        Me.Controls.Add(Me.textBox2)
        Me.Controls.Add(Me.textBox1)
        Me.Controls.Add(Me.button1)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Private WithEvents button2 As System.Windows.Forms.Button
    Private WithEvents label2 As System.Windows.Forms.Label
    Private WithEvents label1 As System.Windows.Forms.Label
    Private WithEvents textBox2 As System.Windows.Forms.TextBox
    Private WithEvents textBox1 As System.Windows.Forms.TextBox
    Private WithEvents button1 As System.Windows.Forms.Button

End Class
