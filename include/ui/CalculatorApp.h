#pragma once

/**
 * @brief Basic four-function calculator (0-9, + - * /, =, C, +/-, .).
 *
 * Stateful: accumulator, pending operator, and the entry string live as
 * members. Instantiated as a member of MockApps — no standalone global.
 */
class CalculatorApp
{
public:
    /**
     * @brief Renders the Calculator window (display + keypad) for this frame.
     *
     * Caller (MockApps) gates on AppState::show_app_1; the flag is passed
     * to ImGui::Begin so the native 'X' toggles it.
     */
    void Render();

private:
    /**
     * @brief Draws the right-aligned result line in a bordered child.
     */
    void RenderDisplay();

    /**
     * @brief Draws the 4-column keypad and routes clicks to the handlers.
     */
    void RenderKeypad();

    /**
     * @brief Appends a digit to the entry (clears an error first).
     *
     * @param digit '0'-'9'. Replaces a lone leading "0"; ignored once the
     *              entry is full.
     */
    void InputDigit(char digit);

    /**
     * @brief Appends a decimal point when the entry has none.
     *
     * Starts from "0." on a fresh entry.
     */
    void InputDot();

    /**
     * @brief Commits the pending operation and arms the new operator.
     *
     * Left-to-right evaluation, no precedence. Operators are replaced
     * when pressed back-to-back.
     *
     * @param op One of '+' '-' '*' '/'.
     */
    void InputOp(char op);

    /**
     * @brief Evaluates the armed operation (accumulator op entry).
     *
     * No-op when no operator is pending or an error is latched.
     */
    void Equals();

    /**
     * @brief Toggles the entry's sign by adding or removing '-'.
     *
     * "0" stays "0"; no-op while an error is latched.
     */
    void ToggleSign();

    /**
     * @brief Resets every member to its default state, including error.
     */
    void AllClear();

    /**
     * @brief Computes accumulator <pending_op> entry and stores it.
     *
     * Writes the result (via "%.12g") into entry and accumulator.
     * Division by zero latches error and displays "Error".
     */
    void Evaluate();

    double accumulator = 0.0; ///< Left operand of the pending operation.
    char pending_op = 0;      ///< 0 when idle, else '+' '-' '*' '/'.
    char entry[32] = "0";     ///< Current number exactly as typed / result.
    bool fresh_entry = true;  ///< Next digit starts a new entry.
    bool error = false;       ///< Divide-by-zero latch (C or digit clears).
};
