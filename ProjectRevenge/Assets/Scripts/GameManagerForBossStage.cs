using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManagerForBossStage : GameManagerForStage
{
    private Enemy boss;
    public GameObject GameClear;
    public float WaitTime;
    private float stopWatch;

    // Start is called before the first frame update
    private new void Awake()
    {
        base.Awake();

        boss = GameObject.FindGameObjectWithTag("Boss").GetComponent<Enemy>();
        stopWatch = WaitTime;
    }

    // Update is called once per frame
    new void Update()
    {
        base.Update();

        if(boss.mbDead)
        {
            stopWatch -= Time.deltaTime;
            if(stopWatch <= 0)
            {
                GameClear.SetActive(true);
                GamePause(true);
            }
        }
    }
}
