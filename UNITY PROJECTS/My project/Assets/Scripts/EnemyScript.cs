using System;
using UnityEngine;

public class EnemyScript : MonoBehaviour
{
    public int health = 1;
    public int scoreValue = 10;

    public static event Action<int> OnEnemyKilled;

    public void TakeDamage(int damage)
    {
        health -= damage;

        Debug.Log("Enemy damaged. Health = " + health);

        if (health <= 0)
        {
            Debug.Log("Enemy killed! Sending score event.");

            OnEnemyKilled?.Invoke(scoreValue);

            Destroy(gameObject);
        }
    }
}