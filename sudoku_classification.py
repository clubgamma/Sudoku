def classifyDifficulty(clueCount: int, backtrackingSteps: int) -> str:
    """
    Classifies the difficulty of a Sudoku puzzle based on the number of clues and backtracking steps.
    
    Parameters:
        clueCount (int): The number of clues in the Sudoku puzzle.
        backtrackingSteps (int): The number of backtracking steps taken to solve the puzzle.
    
    Returns:
        str: The difficulty level as "Easy", "Medium", "Hard", or "Expert".
    """
    # Initialize score by subtracting clueCount from 40 and adding backtrackingSteps divided by 10
    score = 40 - clueCount + (backtrackingSteps // 10)
    
    # Classify the puzzle difficulty based on the score
    if score < 10:
        return "Easy"
    elif 10 <= score < 20:
        return "Medium"
    elif 20 <= score < 30:
        return "Hard"
    else:
        return "Expert"
