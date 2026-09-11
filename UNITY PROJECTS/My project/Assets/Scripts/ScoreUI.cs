using TMPro;
using UnityEngine;

public class ScoreUI : MonoBehaviour
{
    public TextMeshProUGUI scoreText;

    void Start()
    {
        if (ScoreManager.Instance == null)
        {
            Debug.LogError("ScoreManager does not exist in the scene!");
            return;
        }

        ScoreManager.Instance.OnScoreChanged += UpdateScore;

        UpdateScore(ScoreManager.Instance.Score);
    }

    void OnDestroy()
    {
        if (ScoreManager.Instance != null)
        {
            ScoreManager.Instance.OnScoreChanged -= UpdateScore;
        }
    }

    void UpdateScore(int score)
    {
        scoreText.text = "Score: " + score;

        Debug.Log("Score UI updated: " + score);
    }
}