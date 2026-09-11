using System;
using UnityEngine;

public class ScoreManager : MonoBehaviour
{
    public static ScoreManager Instance;

    public int Score { get; private set; }

    public event Action<int> OnScoreChanged;

    void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
        }
        else
        {
            Destroy(gameObject);
        }
    }

    void OnEnable()
    {
        EnemyScript.OnEnemyKilled += AddScore;
    }

    void OnDisable()
    {
        EnemyScript.OnEnemyKilled -= AddScore;
    }

    void AddScore(int points)
    {
        Score += points;

        Debug.Log("Score changed to: " + Score);

        OnScoreChanged?.Invoke(Score);
    }
}