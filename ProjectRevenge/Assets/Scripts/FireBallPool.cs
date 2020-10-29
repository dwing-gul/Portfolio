using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireBallPool : MonoBehaviour
{
    public FireBall Prefab;
    private List<FireBall> Pool;

    // Start is called before the first frame update
    void Start()
    {
        Pool = new List<FireBall>();
    }

    public FireBall GetFromPool()
    {
        for (int i = 0; i < Pool.Count; i++)
        {
            if (!Pool[i].gameObject.activeInHierarchy)
            {
                Pool[i].gameObject.SetActive(true);
                return Pool[i];
            }
        }

        FireBall newObj = Instantiate(Prefab); //instantiate는 인스턴스 생성이라는 의미를 가진다.(계층란에 새로운 으브젝트를 복제하여 만듬)
        Pool.Add(newObj);
        return newObj;
    }
}
