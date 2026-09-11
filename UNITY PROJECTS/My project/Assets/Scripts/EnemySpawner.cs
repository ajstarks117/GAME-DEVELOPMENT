using UnityEngine;

public class EnemySpawner : MonoBehaviour
{
    public GameObject enemyPrefab;

    public Transform[] spawnPoints;

    void Start()
    {
        SpawnEnemies();
    }

    void SpawnEnemies()
    {
        foreach (Transform spawnPoint in spawnPoints)
        {
            Instantiate(
                enemyPrefab,
                spawnPoint.position,
                spawnPoint.rotation
            );
        }
    }
}