import java.awt.*
import java.awt.event.ActionEvent
import java.awt.event.WindowEvent
import java.awt.event.WindowListener

class DoRequestApp(parent: Frame) : Dialog(parent), WindowListener {
    init {
        initUi()
    }

    private fun initUi() {
        layout = null;
        isResizable = false;
        size = Dimension(640, 480);
        title = "Do Request App";
        addWindowListener(this);

        // Header
        val header: Label = Label("Do Request App")
        header.font = Font("Arial Unicode MS", Font.PLAIN, 32)
        header.setBounds(10, 30, 480, 40)
        add(header)

        // URL
        val uidLabel = Label("URL:")
        uidLabel.setBounds(20, 80, 35, 22)
        add(uidLabel)

        val urlField: TextField = TextField("http://localhost:5000/");
        urlField.bounds = Rectangle(80, 80, 220, 22)
        add(urlField)

        // Content
        val contentLabel = Label("Request content:")
        contentLabel.bounds = Rectangle(20, 110, 220, 22)
        add(contentLabel)

        val contentArea = TextArea("{}")
        contentArea.bounds = Rectangle(20, 140, 600, 280)
        add(contentArea)

        // Send
        val sendButton = Button("Send")
        sendButton.bounds = Rectangle(500, 440, 120, 22)
        add(sendButton)

        // Exit
        val exitButton = Button("Exit")
        exitButton.bounds = Rectangle(20, 440, 120, 22)
        exitButton.addActionListener{ dispose() }
        add(exitButton)
    }

    fun run() {
        isVisible = true;

    }

    override fun windowOpened(e: WindowEvent?) {}

    override fun windowClosing(e: WindowEvent?) {
        dispose()
    }

    override fun windowClosed(e: WindowEvent?) {}

    override fun windowIconified(e: WindowEvent?) {}

    override fun windowDeiconified(e: WindowEvent?) {}

    override fun windowActivated(e: WindowEvent?) {}

    override fun windowDeactivated(e: WindowEvent?) {}
}