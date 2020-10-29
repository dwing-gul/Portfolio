using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GaugeBarPool : MonoBehaviour
{
    public GaugeBar Origin;
    public Transform OriginParents;
    private List<GaugeBar> Pool;

    private void Awake()
    {
        Pool = new List<GaugeBar>();
    }

    public GaugeBar GetFromPool()
    {
        for(int i = 0; i < Pool.Count; i++)
        {
            if(!Pool[i].gameObject.activeInHierarchy)
            {
                Pool[i].gameObject.SetActive(true);
                return Pool[i];
            }
        }

        GaugeBar newObj = Instantiate(Origin, OriginParents);
        Pool.Add(newObj);
        return newObj;
    }
}
