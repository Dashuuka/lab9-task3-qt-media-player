import QtQuick
import QtQuick.Controls
import QtTest
import GuessNumberXTest

TestCase {
    id: tc
    name: "GameCoreTests"
    width: 400
    height: 600
    visible: true
    when: windowShown

    function findChild(root, name) {
        const stack = [root]
        while (stack.length > 0) {
            const item = stack.pop()
            if (item.objectName === name) return item
            for (let i = 0; i < item.children.length; ++i) {
                stack.push(item.children[i])
            }
        }
        return null
    }

    Component {
        id: coreComp
        GameCore { anchors.fill: parent }
    }

    function init() {
        gameVm.newGame()
    }

    function test_0_renders() {
        const obj = createTemporaryObject(coreComp, tc)
        verify(obj)
        wait(50)
        const input = findChild(obj, "guessInput")
        const submit = findChild(obj, "submitBtn")
        verify(input !== null, "guessInput must exist")
        verify(submit !== null, "submitBtn must exist")
    }

    function test_1_submitForwardsToVm() {
        const obj = createTemporaryObject(coreComp, tc)
        wait(50)
        const input = findChild(obj, "guessInput")
        const submit = findChild(obj, "submitBtn")
        verify(input); verify(submit)
        const attemptsBefore = gameVm.currentAttempts
        input.text = "50"
        submit.clicked()
        wait(50)
        verify(gameVm.currentAttempts > attemptsBefore,
               "VM should register a guess")
        compare(input.text, "", "input should clear after submission")
    }

    function test_2_invalidInputShowsError() {
        const obj = createTemporaryObject(coreComp, tc)
        wait(50)
        const err = findChild(obj, "errorLabel")
        verify(err !== null)
        gameVm.submitGuess("zzz")
        wait(80)
        verify(err.text.length > 0, "error label should not be empty")
    }

    function test_3_imageReflectsState() {
        const obj = createTemporaryObject(coreComp, tc)
        wait(50)
        gameVm.newGame()
        wait(50)
        verify(gameVm.imageSource.indexOf("question") >= 0,
               "should show question icon; got " + gameVm.imageSource)
    }

    function test_4_progressTracksAttempts() {
        const obj = createTemporaryObject(coreComp, tc)
        wait(50)
        const before = gameVm.currentAttempts
        gameVm.submitGuessInt(10)
        wait(50)
        verify(gameVm.currentAttempts === before + 1)
    }
}
